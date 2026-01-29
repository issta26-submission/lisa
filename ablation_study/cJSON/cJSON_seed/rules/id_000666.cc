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
//<ID> 666
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
    size_t printed_len = 0;
    size_t to_copy = 0;
    const char *version = NULL;
    size_t version_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *str = cJSON_CreateString("hello");

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate / Validate
    printed1 = cJSON_PrintUnformatted(root);
    printed_len = printed1 ? strlen(printed1) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed1 ? printed1 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed1);
    cJSON_DeleteItemFromArray(arr, 1);
    printed2 = cJSON_PrintUnformatted(root);
    printed_len = printed2 ? strlen(printed2) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed2 ? printed2 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed2);
    version = cJSON_Version();
    version_len = version ? strlen(version) : 0;
    to_copy = version_len < (511 - to_copy) ? version_len : (511 - to_copy);
    memcpy(scratch + to_copy, version ? version : "", to_copy);
    scratch[to_copy + to_copy > 511 ? 511 : to_copy + to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}