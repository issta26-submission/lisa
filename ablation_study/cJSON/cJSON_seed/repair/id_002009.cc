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
//<ID> 2009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON *str_item = cJSON_CreateString("sensor_A");
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "reading", num_item);
    cJSON_AddItemToObject(child, "label", str_item);

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNullToObject(root, "optional");
    cJSON *first_elem = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, first_elem);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(array, child_ref);
    cJSON_AddItemToObject(root, "data", array);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *elem0 = cJSON_GetArrayItem(array, 0);
    cJSON *elem1 = cJSON_GetArrayItem(array, 1);
    cJSON *got_reading = cJSON_GetObjectItem(child, "reading");
    double reading_val = cJSON_GetNumberValue(got_reading);
    char *label_str = cJSON_GetStringValue(cJSON_GetObjectItem(child, "label"));
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (elem0 && elem0->string && elem0->type) ? elem0->string[0] : 'x';
    buffer[2] = (label_str && label_str[0]) ? label_str[0] : 'y';
    buffer[3] = (char)('0' + ((int)reading_val % 10));
    buffer[4] = (char)('0' + (printed_ok % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}