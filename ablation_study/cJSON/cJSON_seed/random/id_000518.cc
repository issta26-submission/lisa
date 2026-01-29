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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int numbers[3];
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Initialize
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(numbers, 3);
    name = cJSON_CreateString("example");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", arr);
    add_ok2 = cJSON_AddItemToObject(root, "name", name);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "ints");
    arr_size = cJSON_GetArraySize(got_arr);
    got_name = cJSON_GetObjectItem(root, "name");

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)got_name;
    (void)arr_size;
    (void)buffer;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}