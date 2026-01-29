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
//<ID> 1999
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
    cJSON *got_item = NULL;
    char *printed = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_obj = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Core operations
    added1 = cJSON_AddItemToArray(arr, str1);
    added2 = cJSON_AddItemToArray(arr, str2);
    added_obj = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate / Inspect
    got_item = cJSON_GetObjectItem(root, "list");
    printed = cJSON_PrintUnformatted(got_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(str2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_obj != 0);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}