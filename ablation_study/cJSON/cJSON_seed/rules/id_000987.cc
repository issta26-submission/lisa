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
//<ID> 987
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    cJSON *values_arr = NULL;
    cJSON *second_value = NULL;
    cJSON *dup_value = NULL;
    cJSON *false_node = NULL;
    char *printed = NULL;
    char *json_buf = NULL;
    const char json_orig[] = " {  \"sensor\" : \"S1\" , \"values\" : [ 10 , false , 20 ] , \"active\" : true } ";
    size_t json_len = (size_t)(sizeof(json_orig) - 1);
    size_t buf_len = json_len + 1;

    // step 2: Setup / Configure
    json_buf = (char *)cJSON_malloc(buf_len);
    memcpy(json_buf, json_orig, buf_len);
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    values_arr = cJSON_GetObjectItem(parsed, "values");
    second_value = cJSON_GetArrayItem(values_arr, 1);
    dup_value = cJSON_Duplicate(second_value, 1);
    false_node = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, dup_value);
    cJSON_AddItemToArray(arr, false_node);
    cJSON_AddItemToObject(root, "sensor_name", cJSON_CreateString(cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "sensor"))));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(json_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}