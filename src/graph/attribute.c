#include "private/attribute.h"
#include <stdlib.h>

void cgraphAttrDestroyList(CGraphAttribute *attr) {
  for (CGraphAttribute *next; attr; attr = next) {
    next = attr->next;
    free(attr->vector);
    free(attr);
  }
}
