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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *printed_dup = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *dup_root = NULL;
    int arr_size = 0;
    size_t root_len = 0;
    size_t dup_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_dup = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddRawToObject(root, "meta", "{\"info\":\"raw\",\"valid\":true}");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    dup_root = cJSON_Duplicate(root, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_dup = cJSON_PrintUnformatted(dup_root);
    root_len = printed_root ? strlen(printed_root) : 0;
    dup_len = printed_dup ? strlen(printed_dup) : 0;
    to_copy_root = root_len < 511 ? root_len : 511;
    to_copy_dup = (dup_len < (511 - to_copy_root)) ? dup_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, printed_dup ? printed_dup : "", to_copy_dup);
    scratch[to_copy_root + to_copy_dup] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}