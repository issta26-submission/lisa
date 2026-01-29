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
//<ID> 1996
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
    cJSON *str1 = NULL;
    cJSON *str2 = NULL;
    cJSON *got_arr = NULL;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    char *printed = NULL;
    int validation = 0;

    // step 2: Setup (create object, array and string items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Core operations (add strings to array, add array to object, retrieve and print)
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "letters", arr);
    got_arr = cJSON_GetObjectItem(root, "letters");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(str2 != NULL);
    validation ^= (int)(added_str1 != 0);
    validation ^= (int)(added_str2 != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}