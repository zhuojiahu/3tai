/****************************************************************************
** Meta object code from reading C++ file 'myimageshowitem.h'
**
** Created: Tue Jun 7 10:15:29 2022
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../myimageshowitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myimageshowitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyImageShowItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      51,   16,   16,   16, 0x05,
      77,   16,   16,   16, 0x05,
     104,   16,   16,   16, 0x05,
     134,   16,   16,   16, 0x05,
     157,   16,   16,   16, 0x05,
     180,   16,   16,   16, 0x05,
     203,   16,   16,   16, 0x05,
     232,   16,   16,   16, 0x05,
     256,   16,   16,   16, 0x05,
     279,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     309,  301,   16,   16, 0x0a,
     411,  393,   16,   16, 0x0a,
     460,  449,   16,   16, 0x2a,
     499,  493,   16,   16, 0x2a,
     528,  393,   16,   16, 0x0a,
     568,  449,   16,   16, 0x2a,
     603,  493,   16,   16, 0x2a,
     634,   16,   16,   16, 0x0a,
     649,   16,   16,   16, 0x0a,
     672,   16,   16,   16, 0x0a,
     697,   16,   16,   16, 0x0a,
     715,   16,   16,   16, 0x0a,
     733,   16,   16,   16, 0x0a,
     751,   16,   16,   16, 0x0a,
     778,   16,   16,   16, 0x0a,
     797,   16,   16,   16, 0x0a,
     825,   16,   16,   16, 0x0a,
     843,   16,   16,   16, 0x0a,
     860,   16,   16,   16, 0x0a,
     881,   16,   16,   16, 0x0a,
     903,   16,   16,   16, 0x0a,
     928,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyImageShowItem[] = {
    "MyImageShowItem\0\0signals_imageItemDoubleClick(int)\0"
    "signals_showPrevious(int)\0"
    "signals_showFollowing(int)\0"
    "signals_showStartRefresh(int)\0"
    "signals_startShowAll()\0signals_showCheck(int)\0"
    "signals_stopCheck(int)\0"
    "signals_stopAllStressCheck()\0"
    "signals_startCheck(int)\0signals_startShow(int)\0"
    "signals_stopShow(int)\0,,,,,,,\0"
    "slots_updateImage(QImage*,QString,QString,QString,QString,QString,QLis"
    "t<QRect>,int)\0"
    "error,time,bError\0"
    "slots_showErrorInfo(QString,int,bool)\0"
    "error,time\0slots_showErrorInfo(QString,int)\0"
    "error\0slots_showErrorInfo(QString)\0"
    "slots_showWarningInfo(QString,int,bool)\0"
    "slots_showWarningInfo(QString,int)\0"
    "slots_showWarningInfo(QString)\0"
    "slots_update()\0slots_clearErrorInfo()\0"
    "slots_clearWarningInfo()\0slots_saveImage()\0"
    "slots_showCheck()\0slots_stopCheck()\0"
    "slots_stopAllStressCheck()\0"
    "slots_startCheck()\0slots_startAllStressCheck()\0"
    "slots_startShow()\0slots_stopShow()\0"
    "slots_showPrevious()\0slots_showFollowing()\0"
    "slots_showStartRefresh()\0slots_startShowAll()\0"
};

void MyImageShowItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyImageShowItem *_t = static_cast<MyImageShowItem *>(_o);
        switch (_id) {
        case 0: _t->signals_imageItemDoubleClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signals_showPrevious((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signals_showFollowing((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->signals_showStartRefresh((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->signals_startShowAll(); break;
        case 5: _t->signals_showCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->signals_stopCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->signals_stopAllStressCheck(); break;
        case 8: _t->signals_startCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->signals_startShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->signals_stopShow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->slots_updateImage((*reinterpret_cast< QImage*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QList<QRect>(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 12: _t->slots_showErrorInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 13: _t->slots_showErrorInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->slots_showErrorInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->slots_showWarningInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 16: _t->slots_showWarningInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->slots_showWarningInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->slots_update(); break;
        case 19: _t->slots_clearErrorInfo(); break;
        case 20: _t->slots_clearWarningInfo(); break;
        case 21: _t->slots_saveImage(); break;
        case 22: _t->slots_showCheck(); break;
        case 23: _t->slots_stopCheck(); break;
        case 24: _t->slots_stopAllStressCheck(); break;
        case 25: _t->slots_startCheck(); break;
        case 26: _t->slots_startAllStressCheck(); break;
        case 27: _t->slots_startShow(); break;
        case 28: _t->slots_stopShow(); break;
        case 29: _t->slots_showPrevious(); break;
        case 30: _t->slots_showFollowing(); break;
        case 31: _t->slots_showStartRefresh(); break;
        case 32: _t->slots_startShowAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyImageShowItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyImageShowItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MyImageShowItem,
      qt_meta_data_MyImageShowItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyImageShowItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyImageShowItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyImageShowItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyImageShowItem))
        return static_cast<void*>(const_cast< MyImageShowItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int MyImageShowItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void MyImageShowItem::signals_imageItemDoubleClick(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyImageShowItem::signals_showPrevious(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyImageShowItem::signals_showFollowing(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyImageShowItem::signals_showStartRefresh(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MyImageShowItem::signals_startShowAll()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MyImageShowItem::signals_showCheck(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MyImageShowItem::signals_stopCheck(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MyImageShowItem::signals_stopAllStressCheck()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void MyImageShowItem::signals_startCheck(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MyImageShowItem::signals_startShow(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MyImageShowItem::signals_stopShow(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
