#pragma once
#include "../oxygine-include.h"

#include <atomic>

#include <qobject.h>

namespace oxygine
{
    /** RefCounter **/
    class ref_counter
    {
    public:
        std::atomic<qint32> _ref_counter{0};

        ref_counter()
            : _ref_counter(0)
        {
        }
        virtual ~ref_counter()
        {
        }

        void addRef()
        {
            _ref_counter++;
        }

        void releaseRef()
        {
            _ref_counter--;
            Q_ASSERT(_ref_counter >= 0);
            if (_ref_counter <= 0)
            {
                QObject* pObj = dynamic_cast<QObject*>(this);
                if (pObj == nullptr)
                {
                    delete this;
                }
                else
                {
                    pObj->deleteLater();
                }
            }
        }

    private:
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete ;
        ref_counter(const ref_counter&&) = delete ;
        const ref_counter&& operator=(const ref_counter&&) = delete ;
    };
}
