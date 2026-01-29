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
//<ID> 1992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *got_letters = NULL;
    cJSON *got_greet = NULL;
    char *out = NULL;
    const char *greet_value = NULL;
    cJSON_bool add_s1_to_array = 0;
    cJSON_bool add_s2_to_array = 0;
    cJSON_bool add_arr_to_root = 0;
    cJSON_bool add_greet = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Core operations
    add_s1_to_array = cJSON_AddItemToArray(arr, s1);
    add_s2_to_array = cJSON_AddItemToArray(arr, s2);
    add_arr_to_root = cJSON_AddItemToObject(root, "letters", arr);
    add_greet = cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_letters = cJSON_GetObjectItem(root, "letters");
    got_greet = cJSON_GetObjectItem(root, "greeting");
    greet_value = cJSON_GetStringValue(got_greet);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(add_s1_to_array != 0);
    validation ^= (int)(add_s2_to_array != 0);
    validation ^= (int)(add_arr_to_root != 0);
    validation ^= (int)(add_greet != 0);
    validation ^= (int)(got_letters != NULL);
    validation ^= (int)(got_greet != NULL);
    validation ^= (int)(greet_value != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    if (out) { cJSON_free(out); } /* harmless single conditional for safe free */
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}