#ifndef SUPPORT_H
#define SUPPORT_H

#include <QByteArray>
#include <openssl/rand.h>
#include <QRandomGenerator>
#include <QVariant>

QByteArray randomKey(void);
QByteArray randomHeaderNonce(void);
// Random char to add slice_uuid
QString generateRandomLetters(int len = 6);


#endif // SUPPORT_H
