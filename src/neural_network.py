import tensorflow as tf

from tensorflow.python.data.ops.dataset_ops import AUTOTUNE
from tensorflow.keras import layers
from tensorflow.keras import Sequential
from tensorflow.keras import applications

import os

os.environ["KERAS_BACKEND"] = "plaidml.keras.backend"

config = {}
IMG_SHAPE = (224, 224, 3)


def read_config():
    file = open("../config/config.cfg", "r")
    for line in file:
        if line[0] == '#' or line[0] == '\n':
            continue
        key, value = line.split("=")
        config[key] = value.rstrip()


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


def get_model():
    base_model = applications.MobileNetV2(input_shape=IMG_SHAPE,
                                          include_top=False,
                                          weights='imagenet')
    base_model.trainable = False

    model = Sequential([
        base_model,
        layers.GlobalAveragePooling2D(),
        layers.Dense(1024, activation='relu'),
        layers.Dense(512, activation='relu'),
        layers.Dense(13, activation='softmax')  # 5
    ])

    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])

    model.summary()

    return model


def main():
    read_config()
    train_ds = load_data("TRAINING_CSV")
    val_ds = load_data("TESTING_CSV")
    train_ds = preprocess_data(train_ds, True)
    val_ds = preprocess_data(val_ds, False)

    for img, label in train_ds.take(1):
        print("Shape: ", img.numpy().shape)
        print("Labels: ", label.numpy())

    model = get_model()

    epochs = int(config["EPOCH"])
    history = model.fit(
        train_ds,
        validation_data=val_ds,
        epochs=epochs
    )

    loss, acc = model.evaluate(val_ds)
    print("Loss", loss, "Acc", acc)
    return


main()
