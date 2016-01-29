/****************************************************************************
** Meta object code from reading C++ file 'image_editor_widget_class.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/image_editor_widget_class.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'image_editor_widget_class.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_image_editor_widget_t {
    QByteArrayData data[8];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_image_editor_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_image_editor_widget_t qt_meta_stringdata_image_editor_widget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "image_editor_widget"
QT_MOC_LITERAL(1, 20, 5), // "hello"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 32), // "show_horizontal_scroll_bar_stuff"
QT_MOC_LITERAL(4, 60, 30), // "show_vertical_scroll_bar_stuff"
QT_MOC_LITERAL(5, 91, 19), // "show_geometry_stuff"
QT_MOC_LITERAL(6, 111, 9), // "save_file"
QT_MOC_LITERAL(7, 121, 12) // "save_file_as"

    },
    "image_editor_widget\0hello\0\0"
    "show_horizontal_scroll_bar_stuff\0"
    "show_vertical_scroll_bar_stuff\0"
    "show_geometry_stuff\0save_file\0"
    "save_file_as"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_image_editor_widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x09 /* Protected */,
       3,    0,   45,    2, 0x09 /* Protected */,
       4,    0,   46,    2, 0x09 /* Protected */,
       5,    0,   47,    2, 0x09 /* Protected */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void image_editor_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        image_editor_widget *_t = static_cast<image_editor_widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hello(); break;
        case 1: _t->show_horizontal_scroll_bar_stuff(); break;
        case 2: _t->show_vertical_scroll_bar_stuff(); break;
        case 3: _t->show_geometry_stuff(); break;
        case 4: _t->save_file(); break;
        case 5: _t->save_file_as(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject image_editor_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_image_editor_widget.data,
      qt_meta_data_image_editor_widget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *image_editor_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *image_editor_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_image_editor_widget.stringdata0))
        return static_cast<void*>(const_cast< image_editor_widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int image_editor_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
