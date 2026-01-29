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
//<ID> 616
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
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *dup_root = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddRawToObject(root, "raw", "{\"note\":\"data\",\"flag\":true}");
    cJSON_AddNumberToObject(child, "count", 2.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_dup = cJSON_PrintUnformatted(dup_root);
    arr_size = cJSON_GetArraySize(arr);
    memcpy(scratch, printed_root ? printed_root : "", printed_root ? (strlen(printed_root) < 255 ? strlen(printed_root) : 255) : 0);
    scratch[ (printed_root ? (strlen(printed_root) < 255 ? strlen(printed_root) : 255) : 0) ] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}