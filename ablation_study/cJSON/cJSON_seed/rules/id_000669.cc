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
//<ID> 669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed1 = NULL;
    char *printed2 = NULL;
    const char *version = NULL;
    size_t printed1_len = 0;
    size_t printed2_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON *item_c = cJSON_CreateString("gamma");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToArray(arr, item_c);

    // step 3: Operate / Validate
    printed1 = cJSON_PrintUnformatted(root);
    printed1_len = printed1 ? strlen(printed1) : 0;
    to_copy = printed1_len < 255 ? printed1_len : 255;
    memcpy(scratch, printed1 ? printed1 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed1);
    cJSON_DeleteItemFromArray(arr, 1);
    printed2 = cJSON_Print(root);
    printed2_len = printed2 ? strlen(printed2) : 0;
    to_copy = printed2_len < 255 ? printed2_len : 255;
    memset(scratch, 0, 256);
    memcpy(scratch, printed2 ? printed2 : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed2);
    version = cJSON_Version();
    memcpy(scratch + 128, version ? version : "", strlen(version ? version : ""));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}