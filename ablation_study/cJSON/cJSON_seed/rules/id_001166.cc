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
//<ID> 1166
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *t = NULL;
    cJSON *f = NULL;
    cJSON *single_item = NULL;
    char namebuf[16];
    int arr_size = 0;
    double val_n1 = 0.0;
    double val_n2 = 0.0;
    double val_single = 0.0;

    // step 2: Setup / Configure
    memset(namebuf, 0, sizeof(namebuf));
    namebuf[0] = 'v'; namebuf[1] = 'a'; namebuf[2] = 'l'; namebuf[3] = '\0';
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.5);
    n2 = cJSON_CreateNumber(-3.14);
    t = cJSON_CreateTrue();
    f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddNumberToObject(root, "single", 7.0);
    cJSON_AddStringToObject(root, namebuf, "label");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    val_n1 = cJSON_GetNumberValue(n1);
    val_n2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    single_item = cJSON_GetObjectItem(root, "single");
    val_single = cJSON_GetNumberValue(single_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}