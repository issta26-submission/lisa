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
//<ID> 663
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
    cJSON *str_item = cJSON_CreateString("first_item");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *false_item = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    const char *version = NULL;
    size_t version_len = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;
    size_t offset = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    false_item = cJSON_AddFalseToObject(root, "enabled");
    add_ok1 = cJSON_AddItemToArray(arr, str_item);
    add_ok2 = cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate / Validate
    version = cJSON_Version();
    version_len = version ? strlen(version) : 0;
    to_copy = version_len < 511 ? version_len : 511;
    memcpy(scratch, version ? version : "", to_copy);
    offset = to_copy;
    scratch[offset < 511 ? offset : 511] = '\0';
    printed1 = cJSON_PrintUnformatted(root);
    printed_len = printed1 ? strlen(printed1) : 0;
    to_copy = printed_len < (511 - offset) ? printed_len : (511 - offset);
    memcpy(scratch + offset, printed1 ? printed1 : "", to_copy);
    offset += to_copy;
    scratch[offset < 511 ? offset : 511] = '\0';
    cJSON_free(printed1);
    cJSON_DeleteItemFromArray(arr, 0);
    printed2 = cJSON_PrintUnformatted(root);
    cJSON_free(printed2);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}