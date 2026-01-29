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
//<ID> 1991
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
    cJSON *got_arr = NULL;
    char *json_str = NULL;
    cJSON_bool added_arr_item1 = 0;
    cJSON_bool added_arr_item2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Core operations
    added_arr_item1 = cJSON_AddItemToArray(arr, s1);
    added_arr_item2 = cJSON_AddItemToArray(arr, s2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("set"));

    // step 4: Inspect and serialize
    got_arr = cJSON_GetObjectItem(root, "items");
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(added_arr_item1 != 0);
    validation ^= (int)(added_arr_item2 != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(json_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}