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
//<ID> 723
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
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *root = NULL;
    cJSON *trueA = NULL;
    cJSON *trueB = NULL;
    cJSON *trueC = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok1 = 0;
    cJSON_bool replace_ok2 = 0;
    cJSON_bool flag_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    trueA = cJSON_CreateTrue();
    add_ok = cJSON_AddItemToObject(root, "flag", trueA);

    // step 3: Operate / Validate
    trueB = cJSON_CreateTrue();
    replace_ok1 = cJSON_ReplaceItemViaPointer(root, trueA, trueB);
    trueC = cJSON_CreateTrue();
    replace_ok2 = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", trueC);
    got_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_true = cJSON_IsTrue(got_flag);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}