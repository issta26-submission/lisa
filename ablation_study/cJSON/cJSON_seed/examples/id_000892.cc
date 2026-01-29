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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    /* Use a string item (rather than raw) so cJSON_GetStringValue returns a non-NULL pointer. */
    cJSON *raw_item = cJSON_CreateString("{\"sensor\":\"A\",\"value\":7}");
    cJSON *first_number = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, first_number);
    cJSON_AddItemToObject(root, "payload", array);

    // step 3: Operate & Validate
    const char *raw_str = cJSON_GetStringValue(raw_item);
    int size_before = cJSON_GetArraySize(array);
    cJSON *size_number = cJSON_CreateNumber((double)size_before);
    cJSON *raw_length_number = cJSON_CreateNumber((double)strlen(raw_str));
    cJSON_AddItemToObject(root, "payload_size", size_number);
    cJSON_AddItemToObject(root, "raw_length", raw_length_number);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
}