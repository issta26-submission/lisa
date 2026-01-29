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
//<ID> 2134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *got_list = NULL;
    cJSON *got_single = NULL;
    cJSON *got_first = NULL;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool is_num_single = 0;
    cJSON_bool is_num_first = 0;
    double v_single = 0.0;
    double v_first = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    added0 = cJSON_AddItemToArray(arr, n0);
    added1 = cJSON_AddItemToObject(root, "single", n1);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate
    got_list = cJSON_GetObjectItemCaseSensitive(root, "list");
    got_single = cJSON_GetObjectItemCaseSensitive(root, "single");
    got_first = cJSON_GetArrayItem(got_list, 0);
    is_num_single = cJSON_IsNumber(got_single);
    is_num_first = cJSON_IsNumber(got_first);
    v_single = cJSON_GetNumberValue(got_single);
    v_first = cJSON_GetNumberValue(got_first);
    buf[0] = (char)(is_num_single + '0');
    buf[1] = (char)(is_num_first + '0');

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(added0 != 0);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(got_list != NULL);
    validation ^= (int)(got_single != NULL);
    validation ^= (int)(got_first != NULL);
    validation ^= (int)(is_num_single != 0);
    validation ^= (int)(is_num_first != 0);
    validation ^= (int)(v_single == 2.5);
    validation ^= (int)(v_first == 1.5);
    buf[2] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}