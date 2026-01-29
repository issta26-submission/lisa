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
//<ID> 704
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
    cJSON *root = cJSON_CreateObject();
    cJSON *mirror = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *true_item2 = cJSON_CreateTrue();
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *false_item = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool equal_before = 0;
    cJSON_bool equal_after = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag_true", true_item);
    false_item = cJSON_AddFalseToObject(root, "flag_false");
    num1 = cJSON_AddNumberToObject(root, "value", 123.0);
    cJSON_AddItemToObject(mirror, "flag_true", true_item2);
    cJSON_AddFalseToObject(mirror, "flag_false");
    num2 = cJSON_AddNumberToObject(mirror, "value", 123.0);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    equal_before = cJSON_Compare(root, mirror, 1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_ReplaceItemInObject(mirror, "flag_false", cJSON_CreateTrue());
    equal_after = cJSON_Compare(root, mirror, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    return 66;
}