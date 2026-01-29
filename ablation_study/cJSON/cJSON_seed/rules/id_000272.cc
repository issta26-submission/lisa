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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *raw_json = (char *)cJSON_malloc(128);
    memset(raw_json, 0, 128);
    const char *json_template = "{\n  \"numbers\": [ 1.5 , 2.5 , 3.5 ]\n}";
    memcpy(raw_json, json_template, strlen(json_template) + 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.5);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "numbers");

    // step 3: Operate / Validate
    int orig_size = cJSON_GetArraySize(arr);
    int parsed_size = cJSON_GetArraySize(parsed_arr);
    cJSON *first_item = cJSON_GetArrayItem(parsed_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "count_programmatic", (double)orig_size);
    cJSON_AddNumberToObject(root, "count_parsed", (double)parsed_size);
    cJSON_AddNumberToObject(root, "first_value_copy", first_val);

    // step 4: Cleanup
    cJSON_free(raw_json);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}