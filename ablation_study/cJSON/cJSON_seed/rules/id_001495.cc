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
//<ID> 1495
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
    cJSON *bool_item = NULL;
    cJSON *str1 = NULL;
    cJSON *str2 = NULL;
    cJSON *embedded = NULL;
    char json[] = " { \"x\" : 42, \"flag\" : false } ";
    char buf[128];
    int arr_size = 0;
    double xval = 0.0;
    memset(buf, 0, sizeof(buf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    bool_item = cJSON_AddBoolToObject(root, "ok", 1);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "embedded", parsed);
    str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str1);
    str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str2);

    // step 3: Operate / Validate
    embedded = cJSON_GetObjectItem(root, "embedded");
    xval = cJSON_GetNumberValue(cJSON_GetObjectItem(embedded, "x"));
    arr_size = cJSON_GetArraySize(arr);
    (void)buf;
    (void)xval;
    (void)arr_size;
    (void)bool_item;
    (void)embedded;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}