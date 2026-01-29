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
//<ID> 706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *cmp = cJSON_CreateObject();
    cJSON *num_item = NULL;
    cJSON *num_item_cmp = NULL;
    cJSON *true_item = NULL;
    cJSON *true_item_cmp = NULL;
    cJSON *false_item = NULL;
    cJSON *false_item_cmp = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool equal = 0;
    const cJSON *got_true = NULL;

    // step 2: Setup / Configure
    num_item = cJSON_AddNumberToObject(root, "value", 123.0);
    num_item_cmp = cJSON_AddNumberToObject(cmp, "value", 123.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_true", true_item);
    true_item_cmp = cJSON_CreateTrue();
    cJSON_AddItemToObject(cmp, "flag_true", true_item_cmp);
    false_item = cJSON_AddFalseToObject(root, "flag_false");
    false_item_cmp = cJSON_AddFalseToObject(cmp, "flag_false");

    // step 3: Operate / Validate
    got_true = cJSON_GetObjectItem(root, "flag_true");
    is_true = cJSON_IsTrue(got_true);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    equal = cJSON_Compare(root, cmp, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(cmp);

    // API sequence test completed successfully
    return 66;
}