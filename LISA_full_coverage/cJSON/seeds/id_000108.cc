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
//<ID> 108
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
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON_AddItemToArray(arr, n1);
    cJSON *s0 = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(arr, s0);
    cJSON *s2 = cJSON_CreateString("elem2");
    cJSON_AddItemToArray(arr, s2);
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, ins);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    int size = cJSON_GetArraySize(arr);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    char *val1 = cJSON_GetStringValue(item1);
    char *printed2 = cJSON_Print(root);
    (void)flag;
    (void)insert_ok;
    (void)size;
    (void)val1;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}