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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateArray();
    double numbers[2] = {3.14, 2.71};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(numbers, 2);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(container, dbl_arr);
    cJSON_AddItemToArray(container, str_item);
    cJSON_AddItemToArray(container, false_item);
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("my_document"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "container");
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    int arr_size = cJSON_GetArraySize(detached);
    cJSON *first_element = cJSON_GetArrayItem(detached, 0);
    cJSON *first_number_item = cJSON_GetArrayItem(first_element, 0);
    double first_number = cJSON_GetNumberValue(first_number_item);
    cJSON *parsed_title = cJSON_GetObjectItemCaseSensitive(parsed, "title");
    const char *title_value = cJSON_GetStringValue(parsed_title);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    (void)detached_is_array;
    (void)arr_size;
    (void)first_number;
    (void)title_value;
    // API sequence test completed successfully
    return 66;
}