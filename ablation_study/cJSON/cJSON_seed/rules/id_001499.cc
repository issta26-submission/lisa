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
//<ID> 1499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *greet = NULL;
    cJSON *arr_first = NULL;
    cJSON *embedded = NULL;
    cJSON *nested_x = NULL;
    const char *greet_val = NULL;
    int arr_size = 0;
    cJSON_bool printed_ok = 0;
    char json[] = " { \"embedded\" : false , \"nested\" : { \"x\" : true } } ";
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddBoolToObject(root, "active", 1);
    greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", greet);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);

    // step 3: Operate / Validate
    embedded = cJSON_GetObjectItem(parsed, "embedded");
    nested_x = cJSON_GetObjectItem(parsed, "nested");
    nested_x = cJSON_GetObjectItem(nested_x, "x");
    arr_size = cJSON_GetArraySize(arr);
    arr_first = cJSON_GetArrayItem(arr, 0);
    greet_val = cJSON_GetStringValue(greet);
    (void)embedded;
    (void)nested_x;
    (void)arr_size;
    (void)arr_first;
    (void)greet_val;
    (void)printed_ok;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}