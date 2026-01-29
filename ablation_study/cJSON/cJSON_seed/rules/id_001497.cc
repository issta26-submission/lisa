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
//<ID> 1497
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
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *parsed = NULL;
    cJSON *bool_active = NULL;
    cJSON *bool_enabled = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *arr_item1 = NULL;
    char json[] = " { \"embedded\" : true , \"value\" : 100 } ";
    char prebuf[64];
    memset(prebuf, 0, sizeof(prebuf));
    cJSON_bool has_embedded = 0;
    int arr_size = 0;
    double val0 = 0.0;
    const char *sval1 = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    bool_active = cJSON_AddBoolToObject(root, "active", 1);
    bool_enabled = cJSON_AddBoolToObject(root, "enabled", 0);
    num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(arr, num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);
    arr_size = cJSON_GetArraySize(arr);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    arr_item1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(arr_item0);
    sval1 = cJSON_GetStringValue(arr_item1);
    has_embedded = cJSON_HasObjectItem(parsed, "embedded");
    (void)prebuf;
    (void)arr_size;
    (void)val0;
    (void)sval1;
    (void)has_embedded;
    (void)bool_active;
    (void)bool_enabled;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}