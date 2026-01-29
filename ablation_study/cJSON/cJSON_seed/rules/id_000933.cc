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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *alpha = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    const char *s_alpha = "alpha_value";
    int invalid_before = 0;
    int invalid_after = 0;
    int printed_first_char = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    alpha = cJSON_CreateString(s_alpha);
    cJSON_AddItemToObject(root, "alpha", alpha);
    cJSON_AddStringToObject(root, "beta", "beta_value");

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "alpha");
    invalid_before = (int)cJSON_IsInvalid(retrieved);
    printed = cJSON_PrintUnformatted(root);
    printed_first_char = printed ? (int)printed[0] : 0;
    cJSON_DeleteItemFromObject(root, "beta");
    invalid_after = (int)cJSON_IsInvalid(cJSON_GetObjectItem(root, "beta"));

    // step 4: Cleanup
    (void)printed_first_char;
    (void)invalid_before;
    (void)invalid_after;
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}