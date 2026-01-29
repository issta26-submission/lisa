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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("elem1");
    cJSON_AddItemToArray(arr, str);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, inserted);

    // step 3: Operate and Validate
    char json[] = "  { \"x\" : [ 1, 2, 3 ] }  ";
    cJSON_Minify(json);
    cJSON *parsed = cJSON_Parse(json);
    char *printed = cJSON_Print(root);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_bool parsed_is_obj = cJSON_IsObject(parsed);
    int summary = (int)insert_ok + (int)arr_is_array + arr_size + (int)parsed_is_obj;
    (void)summary;
    (void)flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}