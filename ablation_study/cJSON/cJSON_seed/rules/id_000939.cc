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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greet_node = NULL;
    cJSON *retrieved_greet = NULL;
    char *pre_print = NULL;
    char *post_print = NULL;
    char *scratch = NULL;
    const char *greet_text = "hello";
    const char *target_text = "world";
    const char *retrieved_str = NULL;
    cJSON_bool is_invalid = 0;
    int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greet_node = cJSON_CreateString(greet_text);
    cJSON_AddItemToObject(root, "greet", greet_node);
    cJSON_AddStringToObject(root, "target", target_text);

    // step 3: Operate / Validate
    retrieved_greet = cJSON_GetObjectItem(root, "greet");
    retrieved_str = cJSON_GetStringValue(retrieved_greet);
    is_invalid = cJSON_IsInvalid(retrieved_greet);
    pre_print = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str[0];
    scratch[1] = (char)('0' + (int)is_invalid);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "greet");
    post_print = cJSON_PrintUnformatted(root);
    cJSON_free(pre_print);
    cJSON_free(post_print);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}