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
//<ID> 2034
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
    cJSON *got0 = NULL;
    char *setval = NULL;
    const char *val0 = NULL;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    int size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Configure
    setval = cJSON_SetValuestring(s1, "one");
    added0 = cJSON_AddItemToArray(arr, s1);
    added1 = cJSON_AddItemToArray(arr, s2);
    added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    got_arr = cJSON_GetObjectItem(root, "items");
    got0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetStringValue(got0);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(setval != NULL);
    validation ^= (int)(added0 != 0);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(got_arr == arr);
    validation ^= (int)(size == 2);
    validation ^= (int)(val0 != NULL);
    validation ^= (int)(val0 == setval);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}