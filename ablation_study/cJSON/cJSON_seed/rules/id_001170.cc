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
//<ID> 1170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *string_array = NULL;
    cJSON *num_item = NULL;
    cJSON *first_elem = NULL;
    cJSON *first_word_copy = NULL;
    cJSON *copied_num = NULL;
    char *printed = NULL;
    char *first_word_cstr = NULL;
    double new_num = 0.0;
    const char *words[] = { "alpha", "beta", "gamma" };

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    string_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", string_array);
    num_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "count", num_item);

    // step 3: Operate / Validate
    first_elem = cJSON_GetArrayItem(string_array, 0);
    first_word_cstr = cJSON_GetStringValue(first_elem);
    first_word_copy = cJSON_CreateString(first_word_cstr);
    cJSON_AddItemToObject(root, "first_word_copy", first_word_copy);
    new_num = cJSON_SetNumberHelper(num_item, 99.5);
    copied_num = cJSON_CreateNumber(new_num);
    cJSON_AddItemToObject(root, "copied_count", copied_num);
    cJSON_DeleteItemFromArray(string_array, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}