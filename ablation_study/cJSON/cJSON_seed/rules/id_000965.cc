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
//<ID> 965
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
    cJSON *meta = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    double first_value = 0.0;
    int arr_count = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddRawToObject(root, "raw", "{\"embedded\":true}");
    meta = cJSON_CreateNumber(7.0);
    cJSON_bool add_ok = cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_count = cJSON_GetArraySize(arr);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    scratch = (char *)cJSON_malloc((size_t)16);
    memset(scratch, 0, (size_t)16);
    scratch[0] = (char)('0' + (int)arr_count % 10);
    scratch[1] = (char)('0' + (int)first_value % 10);
    (void)first_value;
    (void)add_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}