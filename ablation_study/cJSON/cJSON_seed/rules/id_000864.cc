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
//<ID> 864
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
    cJSON *pi_item = NULL;
    cJSON *num_item = NULL;
    cJSON *str_ref = NULL;
    char *printed = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    int arr_size = 0;
    double got_pi = 0.0;
    const char *str_val = NULL;
    cJSON_bool ok1 = 0;
    cJSON_bool ok2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    str_ref = cJSON_CreateStringReference("hello_ref");
    ok1 = cJSON_InsertItemInArray(arr, 0, str_ref);
    num_item = cJSON_CreateNumber(42.0);
    ok2 = cJSON_InsertItemInArray(arr, 1, num_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_pi = cJSON_GetNumberValue(pi_item);
    str_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (arr_size % 10));
    (void)ok1;
    (void)ok2;
    (void)got_pi;
    (void)str_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}