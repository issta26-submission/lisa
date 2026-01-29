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
//<ID> 707
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
    cJSON *root = cJSON_CreateObject();
    cJSON *other = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "value", 123.0);
    cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToObject(other, "value", cJSON_CreateNumber(123.0));
    cJSON_AddItemToObject(other, "flag", true_item);

    // step 3: Operate / Validate
    cJSON *root_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *other_flag = cJSON_GetObjectItem(other, "flag");
    cJSON_bool root_flag_true = cJSON_IsTrue(root_flag);
    cJSON_bool other_flag_true = cJSON_IsTrue(other_flag);
    cJSON_bool objects_equal = cJSON_Compare(root, other, 1);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}