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
//<ID> 1342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"alpha\": 42, \"beta\": \"hello\"}";
    cJSON *root = NULL;
    cJSON *alpha = NULL;
    cJSON *beta = NULL;
    cJSON *flag = NULL;
    char *printed_before = NULL;
    char *printed_after = NULL;
    double alpha_val = 0.0;
    const char *beta_str = NULL;
    char first_char = 0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    alpha = cJSON_GetObjectItem(root, "alpha");
    alpha_val = cJSON_GetNumberValue(alpha);
    beta = cJSON_GetObjectItem(root, "beta");
    beta_str = cJSON_GetStringValue(beta);
    first_char = beta_str[0];
    printed_before = cJSON_PrintBuffered(root, 128, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "beta");
    printed_after = cJSON_PrintBuffered(root, 128, 1);
    cmp = cJSON_Compare(root, root, 1);
    alpha_val += (double)first_char + (double)cmp;
    (void)alpha_val;

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}