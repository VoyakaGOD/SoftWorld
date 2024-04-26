#include <iostream>
#include <system_error>
#include <QFileDialog>
#include <QMessageBox>

#include "serialize.h"

#include "serialize_ui.h"

bool guiSerializeObj(QWidget* parent, SerializableObject &obj) {
    try {
        QString filename =QFileDialog::getSaveFileName(parent, "Open file");
        if (filename.isNull()) return false;

        FileWriteInterface file_writer = FileWriteInterface(filename.toUtf8().data());
        serializeObj(file_writer, obj);
        return true;
    }
    catch (std::system_error err) {
        std::cout << err.what() << std::endl;
        QMessageBox::critical(parent, "Error", err.what());
        return false;
    }
}

bool guiDeserializeObjInplace(QWidget* parent, SerializableObject *obj
        , int (*func)(SerializableObject*, DataStorageReader&)) {

    try {
        QString filename =QFileDialog::getOpenFileName(parent, "Open file");
        if (filename.isNull()) return false;

        FileReadInterface file_reader = FileReadInterface(filename.toUtf8().data());
        DeserializeError err = (DeserializeError)func(obj, file_reader);
        if (err) {
            QMessageBox::critical(parent, "Error", deserrErrorMsg(err));
            return false;
        }
        return true;
    }
    catch (std::system_error err) {
        std::cout << err.what() << std::endl;
        QMessageBox::critical(parent, "Error", err.what());
        return false;
    }
}
