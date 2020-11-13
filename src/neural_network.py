import tensorflow as tf

tf.get_logger().setLevel('ERROR')

import numpy as np

from tensorflow.python.data.ops.dataset_ops import AUTOTUNE
from tensorflow.keras import layers
from tensorflow.keras import Sequential
from tensorflow.keras import applications
from tensorflow.keras import models

from datetime import datetime

import os
import argparse
import json
import codecs

os.environ["KERAS_BACKEND"] = "plaidml.keras.backend"
config = {}
names = []
IMG_SHAPE = (224, 224, 3)


def init_arg_parser():
    parser = argparse.ArgumentParser(description="Neural Network for face recognition")
    parser.add_argument('--classes', type=int, default=13)
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--train', action='store_true')
    return parser.parse_args()


def read_config():
    file = open("../config/config.cfg", "r")
    for line in file:
        if line[0] == '#' or line[0] == '\n':
            continue
        key, value = line.split("=")
        config[key] = value.rstrip()

    try:
        train_csv = open(config["TRAINING_CSV"], 'r')
        max_class = train_csv.readlines()[-1].split(";")[-1]
    except:
        print("Training csv file not found. Did you create the dataset?")
        exit(1)

    with open(config["LABELS_CSV"], 'r') as labels_file:
        for line in labels_file:
            label, name = line.split(";")
            names.insert(int(label), name)

    file.close()
    train_csv.close()
    return int(max_class) + 1


def load_data(category):
    def process_line(line):
        line_parts = tf.strings.split(line, ";")
        image_path = line_parts[0]
        label = line_parts[1]
        label = tf.strings.to_number(label, out_type=tf.int32)
        image = tf.io.read_file(image_path)
        image = tf.image.decode_jpeg(image, channels=3)
        return image, label

    dataset = tf.data.TextLineDataset(config[category])
    dataset = dataset.map(process_line, num_parallel_calls=AUTOTUNE)

    return dataset


def preprocess_data(dataset, augment=False):
    rescale = Sequential([
        layers.experimental.preprocessing.Resizing(224, 224),
        layers.experimental.preprocessing.Rescaling(1. / 255),
    ])

    augmentation = Sequential([
        layers.experimental.preprocessing.RandomFlip("horizontal_and_vertical"),
        layers.experimental.preprocessing.RandomRotation(0.2),
    ])

    dataset = dataset.map(lambda x, y: (rescale(x, training=True), y),
                          num_parallel_calls=AUTOTUNE)

    dataset = dataset.cache()
    dataset = dataset.shuffle(buffer_size=1000)
    dataset = dataset.batch(int(config["BATCH_SIZE"]))

    if augment:
        dataset = dataset.map(lambda x, y: (augmentation(x, training=True), y),
                              num_parallel_calls=AUTOTUNE)

    dataset = dataset.prefetch(buffer_size=AUTOTUNE)

    return dataset


def create_model():
    base_model = applications.MobileNetV2(input_shape=IMG_SHAPE,
                                          include_top=False,
                                          weights='imagenet')
    base_model.trainable = False

    model = Sequential([
        base_model,
        layers.GlobalAveragePooling2D(),
        layers.Dense(1024, activation='relu'),
        layers.Dense(512, activation='relu'),
        layers.Dense(classes, activation='softmax')
    ])

    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])

    model.summary()

    return model


def train_model():
    train_ds = load_data("TRAINING_CSV")
    val_ds = load_data("TESTING_CSV")
    train_ds = preprocess_data(train_ds, True)
    val_ds = preprocess_data(val_ds, False)

    model = create_model()

    epochs = int(config["EPOCH"])
    history = model.fit(
        train_ds,
        validation_data=val_ds,
        epochs=epochs,
        callbacks=[tensorboard_callback]
    )

    model.save(config["NEURAL_FACE_RECOGNITION_MODEL"])
    print("\nModel saved!")

    with open(config["NEURAL_HISTORY"], 'w') as file:
        json.dump(history.history, file)

    return model


def test_model(model):
    if model == 0:
        # Accuracy 0 due to tensorflow bug #42459
        print("\nLoading model")
        model = models.load_model(config["NEURAL_FACE_RECOGNITION_MODEL"])

    val_ds = load_data("TESTING_CSV")
    val_ds = preprocess_data(val_ds, False)

    print("General evaluation")
    model.evaluate(val_ds,
                   callbacks=[tensorboard_callback]
                   )

    results = model.predict(val_ds, verbose=0)

    image_dim = int(config["WIDTH"])
    std_batch_size = int(config["BATCH_SIZE"])
    correct = 0
    wrong = 0
    batch_count = 0
    with file_writer.as_default():
        for images, labels in val_ds:
            batch_size = images.shape[0]

            for i in range(batch_size):
                name = names[labels[i]]
                predict_name = names[np.argmax(results[ i + (std_batch_size * batch_count) ])]
                image = np.reshape(images[i], (-1, image_dim, image_dim, 3))
                tf.summary.image(str(i) + " Name: " + name + " Predict: " + predict_name, image, max_outputs=25, step=0)

                if name == predict_name:
                    correct += 1
                else:
                    wrong += 1

            batch_count += 1

    print("Total: ", correct+wrong, "Correct: ", correct)
    print("Accuracy: ", correct/(correct+wrong))

    json.dump(results.tolist(),
              codecs.open(config["NEURAL_PREDICT"], 'w', encoding='utf-8'),
              separators=(',', ':'),
              sort_keys=True,
              indent=4
              )


args = init_arg_parser()
classes = read_config()

logdir = config["TENSOR_LOG"] + "/" + datetime.now().strftime("%Y%m%d-%H%M%S")
file_writer = tf.summary.create_file_writer(logdir)
tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir=logdir, histogram_freq=1, profile_batch='5,10')

trained_model = 0
if args.train:
    trained_model = train_model()

if args.test:
    test_model(trained_model)
