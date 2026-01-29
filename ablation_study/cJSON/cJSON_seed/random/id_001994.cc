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
//<ID> 1994
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
    cJSON *name_item = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *got_arr = NULL;
    cJSON_bool added_name = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_item1 = 0;
    cJSON_bool added_item2 = 0;
    char *unformatted = NULL;
    int validation = 0;

    // step 2: Setup (create root object and array, create a name string)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("example");

    // step 3: Core operations (add name to object, add array to object, create strings and add to array)
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateString("two");
    added_item1 = cJSON_AddItemToArray(arr, item1);
    added_item2 = cJSON_AddItemToArray(arr, item2);

    // step 4: Retrieve and print (get the array back via object lookup and produce an unformatted JSON string)
    got_arr = cJSON_GetObjectItem(root, "items");
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(item1 != NULL);
    validation ^= (int)(item2 != NULL);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(added_item1 != 0);
    validation ^= (int)(added_item2 != 0);
    validation ^= (int)(got_arr == arr);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    if (unformatted) { cJSON_free(unformatted); } /* harmless one-line cleanup without branching logic for the test */
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}