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
//<ID> 660
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed1 = NULL;
    char *printed2 = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *extra = cJSON_CreateNumber(7.0);
    const char *version = NULL;
    size_t len_printed1 = 0;
    size_t len_printed2 = 0;
    size_t to_copy = 0;
    size_t ver_len = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, extra);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate / Validate
    printed1 = cJSON_PrintUnformatted(root);
    len_printed1 = printed1 ? strlen(printed1) : 0;
    to_copy = len_printed1 < 511 ? len_printed1 : 511;
    memcpy(scratch, printed1 ? printed1 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed1);
    cJSON_DeleteItemFromArray(arr, 1);
    printed2 = cJSON_Print(root);
    len_printed2 = printed2 ? strlen(printed2) : 0;
    to_copy = len_printed2 < 511 ? len_printed2 : 511;
    memcpy(scratch, printed2 ? printed2 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed2);
    version = cJSON_Version();
    ver_len = version ? strlen(version) : 0;
    to_copy = ver_len < 511 ? ver_len : 511;
    memcpy(scratch + 0, version ? version : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}