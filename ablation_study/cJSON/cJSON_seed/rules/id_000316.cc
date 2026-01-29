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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *tbool = cJSON_AddBoolToObject(root, "flag_true", 1);
    cJSON *fbool = cJSON_AddBoolToObject(root, "flag_false", 0);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.5));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *retrieved_false = cJSON_GetObjectItem(root, "flag_false");
    cJSON *fake = (cJSON *)cJSON_malloc(sizeof(cJSON));
    memset(fake, 0, sizeof(cJSON));
    fake->type = 0;
    cJSON_bool is_false = cJSON_IsFalse(retrieved_false);
    cJSON_bool is_invalid = cJSON_IsInvalid(fake);

    // step 2: Setup / Configure
    (void)tbool;
    (void)fbool;
    (void)arr;

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = (char)('0' + (int)is_invalid);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_free(fake);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}