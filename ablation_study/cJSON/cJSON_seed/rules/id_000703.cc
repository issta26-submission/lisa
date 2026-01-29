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
//<ID> 703
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *other = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *dup = NULL;
    cJSON *got_true = NULL;
    cJSON *false_item = NULL;
    cJSON *num_item = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool are_same = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "truth", true_item);
    false_item = cJSON_AddFalseToObject(other, "lies");
    num_item = cJSON_AddNumberToObject(other, "answer", 42.0);

    // step 3: Operate / Validate
    got_true = cJSON_GetObjectItem(root, "truth");
    is_true = cJSON_IsTrue(got_true);
    dup = cJSON_Duplicate(root, 1);
    are_same = cJSON_Compare(root, dup, 1);
    scratch[0] = (char)('0' + (int)is_true);
    scratch[1] = (char)('0' + (int)are_same);
    scratch[2] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(other);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}