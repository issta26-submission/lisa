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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_num = NULL;
    cJSON *computed_item = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    double computed = 0.0;
    int arr_size = 0;
    cJSON_bool flag_true = 0;
    cJSON_bool added_ref = 0;

    // step 2: Setup / Configure
    num_item = cJSON_AddNumberToObject(root, "num", 42.5);
    flag_item = cJSON_AddTrueToObject(root, "flag");
    cJSON_AddItemToObject(root, "arr", arr);
    added_ref = cJSON_AddItemReferenceToArray(arr, num_item);

    // step 3: Operate / Validate
    got_num = cJSON_GetObjectItem(root, "num");
    num_value = cJSON_GetNumberValue(got_num);
    arr_size = cJSON_GetArraySize(arr);
    flag_true = cJSON_IsTrue(flag_item);
    computed = num_value + (double)arr_size + (double)flag_true + (double)added_ref;
    computed_item = cJSON_AddNumberToObject(root, "computed", computed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}