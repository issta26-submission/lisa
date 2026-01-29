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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *meta_raw = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_values = NULL;
    int arr_size = 0;
    int dup_arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "id", 123.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToObject(root, "values", array);
    meta_raw = cJSON_AddRawToObject(root, "meta", "{\"note\":\"raw\",\"nums\":[1,2,3]}");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    arr_size = cJSON_GetArraySize(array);
    dup_root = cJSON_Duplicate(root, 1);
    dup_values = cJSON_GetObjectItem(dup_root, "values");
    dup_arr_size = cJSON_GetArraySize(dup_values);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}