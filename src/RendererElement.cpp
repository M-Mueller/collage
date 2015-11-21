#include "RendererElement.h"

#include <QtCore/QObject>

void RendererElement::synchronize()
{
    if(auto qobject = dynamic_cast<QObject*>(this))
    {
        for(auto child: qobject->children())
        {
            if(auto subelement = dynamic_cast<RendererElement*>(child))
            {
                subelement->synchronize();
            }
        }
    }
}

