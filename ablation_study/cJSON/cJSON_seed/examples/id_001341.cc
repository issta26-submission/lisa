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
//<ID> 1341
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

    // step 2: Configure
    cJSON *raw_node = cJSON_AddRawToObject(root, "embedded_raw", "{\"inner\":[1,2,3]}");
    cJSON *false_node = cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON *count_node = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_node);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "printed_raw", printed);
    const char *embedded_str = cJSON_GetStringValue(raw_node);
    cJSON *embedded_copy = cJSON_CreateString(embedded_str);
    cJSON_AddItemToObject(root, "embedded_copy", embedded_copy);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}