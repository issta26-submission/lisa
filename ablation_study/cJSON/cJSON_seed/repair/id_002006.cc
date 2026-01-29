#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 2006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "text", str);
    cJSON_AddItemToArray(items, child);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(items, child_ref);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *version = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", version);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *got_num = cJSON_GetObjectItem(first, "num");
    double nval = cJSON_GetNumberValue(got_num);
    cJSON *got_text = cJSON_GetObjectItem(first, "text");
    const char *tstr = cJSON_GetStringValue(got_text);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (size % 10));
    buffer[1] = tstr && tstr[0] ? tstr[0] : 'x';
    buffer[2] = (char)('0' + ((int)nval % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}