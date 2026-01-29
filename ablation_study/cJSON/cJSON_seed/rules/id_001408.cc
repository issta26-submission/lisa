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
//<ID> 1408
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
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *child = NULL;
    char *buffer = NULL;
    const int buf_len = 256;
    cJSON_bool printed_ok = 0;
    cJSON_bool added_to_array1 = 0;
    cJSON_bool added_to_array2 = 0;
    cJSON_bool added_ref_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");
    num_item = cJSON_CreateNumber(100.0);
    added_to_array1 = cJSON_AddItemToArray(arr, str_item);
    added_to_array2 = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "status", "ok");
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "original");
    cJSON_AddItemToObject(root, "orig", child);
    added_ref_ok = cJSON_AddItemReferenceToObject(root, "orig_ref", child);

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    (void)added_to_array1;
    (void)added_to_array2;
    (void)added_ref_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}