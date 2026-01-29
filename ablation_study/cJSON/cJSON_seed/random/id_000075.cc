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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_val = (const char *)0;

    // step 2: Initialize/Configure - attach array and boolean to root
    cJSON_AddItemToObject(root, "words", str_array);
    cJSON_AddItemReferenceToObject(root, "words_ref", str_array);
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Configure - add a numeric property using helper
    num_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate - read back first element from the array and re-add it as a string item
    retrieved_array = cJSON_GetObjectItem(root, "words");
    first_elem = cJSON_GetArrayItem(retrieved_array, 0);
    first_val = cJSON_GetStringValue(first_elem);
    cJSON_AddItemToObject(root, "first_word", cJSON_CreateString(first_val));

    // step 5: Validate - add a derived numeric value (reuse existing number helper)
    cJSON_AddNumberToObject(root, "double_count", num_item->valuedouble * 2.0);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}