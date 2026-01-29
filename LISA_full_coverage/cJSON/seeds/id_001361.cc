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
//<ID> 1361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    float numbers[3] = {1.0f, 2.5f, 3.25f};
    cJSON *float_arr = cJSON_CreateFloatArray(numbers, 3);
    const char *json_literal = "{\"num\":3.14,\"arr\":[4.5,5.5]}";
    size_t json_len = (size_t)(sizeof("{\"num\":3.14,\"arr\":[4.5,5.5]}") - 1);
    cJSON *parsed = cJSON_ParseWithLength(json_literal, json_len);
    cJSON *parsed_num = cJSON_GetObjectItem(parsed, "num");
    cJSON_bool parsed_is_number = cJSON_IsNumber(parsed_num);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemReferenceToObject(root, "floats_ref", float_arr);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON_AddItemReferenceToObject(root, "parsed_arr_ref", parsed_arr);
    int float_count = cJSON_GetArraySize(float_arr);
    cJSON *count_num = cJSON_CreateNumber((double)float_count);
    cJSON_AddItemToObject(root, "float_count", count_num);
    cJSON *isnum_marker = cJSON_CreateNumber((double)parsed_is_number);
    cJSON_AddItemToObject(root, "parsed_num_is_number", isnum_marker);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66;
}