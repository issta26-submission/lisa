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
//<ID> 705
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
    char *printed_root = NULL;
    char *printed_mirror = NULL;
    size_t len_root = 0;
    size_t len_mirror = 0;
    size_t total_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *mirror = cJSON_CreateObject();
    cJSON *num_root = NULL;
    cJSON *num_mirror = NULL;
    cJSON *false_root = NULL;
    cJSON *false_mirror = NULL;
    cJSON *true_root = NULL;
    cJSON *true_mirror = NULL;
    cJSON_bool is_true_root = 0;
    cJSON_bool is_true_mirror = 0;
    cJSON_bool equal_objects = 0;

    // step 2: Setup / Configure
    num_root = cJSON_AddNumberToObject(root, "count", 10.5);
    false_root = cJSON_AddFalseToObject(root, "enabled_false");
    true_root = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled_true", true_root);
    num_mirror = cJSON_AddNumberToObject(mirror, "count", 10.5);
    false_mirror = cJSON_AddFalseToObject(mirror, "enabled_false");
    true_mirror = cJSON_CreateTrue();
    cJSON_AddItemToObject(mirror, "enabled_true", true_mirror);

    // step 3: Operate / Validate
    is_true_root = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled_true"));
    is_true_mirror = cJSON_IsTrue(cJSON_GetObjectItem(mirror, "enabled_true"));
    equal_objects = cJSON_Compare(root, mirror, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_mirror = cJSON_PrintUnformatted(mirror);
    len_root = printed_root ? strlen(printed_root) : 0;
    len_mirror = printed_mirror ? strlen(printed_mirror) : 0;
    total_copy = (len_root + len_mirror) < 511 ? (len_root + len_mirror) : 511;
    memcpy(scratch, printed_root ? printed_root : "", len_root);
    memcpy(scratch + len_root, printed_mirror ? printed_mirror : "", total_copy - len_root);
    scratch[total_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_mirror);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    return 66;
}