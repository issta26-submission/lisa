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
//<ID> 1297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *words = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *shared = cJSON_CreateString("shared");

    // step 2: Configure
    cJSON_AddItemToArray(words, s1);
    cJSON_AddItemToArray(words, s2);
    cJSON_AddItemToObject(root, "words", words);
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);
    cJSON *words_dup = cJSON_Duplicate(words, 1);
    cJSON_AddItemToObject(root, "words_copy", words_dup);

    // step 3: Operate & Validate
    cJSON *flag = cJSON_CreateTrue();
    cJSON_bool is_true = cJSON_IsTrue(flag);
    cJSON *num = cJSON_CreateNumber((double)is_true);
    cJSON_AddItemToObject(root, "flag_numeric", num);
    cJSON *dup_s2 = cJSON_Duplicate(s2, 1);
    cJSON_AddItemToObject(root, "second_dup", dup_s2);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(flag);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}