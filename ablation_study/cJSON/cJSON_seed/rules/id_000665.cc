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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s_item = cJSON_CreateString("alpha");
    cJSON *n_item = cJSON_CreateNumber(3.14159);
    cJSON *null_item = cJSON_CreateNull();
    const char *version = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    size_t ver_len = 0;
    size_t ver_dest = 0;
    size_t to_copy_ver = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s_item);
    cJSON_AddItemToArray(arr, n_item);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *flag_item = cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 1023 ? printed_len : 1023;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    version = cJSON_Version();
    ver_len = version ? strlen(version) : 0;
    ver_dest = to_copy + 1;
    ver_dest = ver_dest < 1023 ? ver_dest : 1023;
    to_copy_ver = ver_len < (1023 - ver_dest) ? ver_len : (1023 - ver_dest);
    memcpy(scratch + ver_dest, version ? version : "", to_copy_ver);
    scratch[ver_dest + to_copy_ver] = '\0';
    cJSON_DeleteItemFromArray(arr, 1);
    (void)flag_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}