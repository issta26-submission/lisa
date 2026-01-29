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
//<ID> 106
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
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, first);
    cJSON *second = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, second);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, inserted);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char json_buf[] = " { \"whitespace\" : true, \"num\" : 2 } ";
    cJSON_Minify(json_buf);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *mid_item = cJSON_GetArrayItem(arr, 1);
    const char *mid_str = cJSON_GetStringValue(mid_item);
    int summary = (int)insert_ok + arr_size + (int)(mid_str ? mid_str[0] : 0) + (int)(printed ? printed[0] : 0);
    (void)flag;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}