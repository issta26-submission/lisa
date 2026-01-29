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
//<ID> 944
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *item_alpha = NULL;
    cJSON *item_beta = NULL;
    cJSON *item_cs = NULL;
    cJSON *item_reg = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *alpha = "alpha";
    const char *beta = "beta";
    const char *cs_value = "caseSensitiveValue";
    const char *reg_value = "regularValue";
    int array_size = 0;
    size_t scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    item_alpha = cJSON_CreateString(alpha);
    cJSON_AddItemToArray(array, item_alpha);
    item_beta = cJSON_CreateString(beta);
    cJSON_AddItemToArray(array, item_beta);
    cJSON_AddItemToObject(root, "letters", array);
    item_cs = cJSON_CreateString(cs_value);
    cJSON_AddItemToObjectCS(root, "CaseKey", item_cs);
    item_reg = cJSON_CreateString(reg_value);
    cJSON_AddItemToObject(root, "regularKey", item_reg);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    array_size = cJSON_GetArraySize(array);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = (printed != NULL && printed[0] != '\0') ? printed[0] : ' ';
    scratch[1] = (char)('0' + (array_size % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}