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
//<ID> 497
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
    cJSON *num_in_array = (cJSON *)0;
    cJSON *num_in_object = (cJSON *)0;
    char buffer[256];
    cJSON_bool add_ok = 0;
    cJSON_bool print_ok = 0;

    // step 2: Initialize
    memset(buffer, 0, sizeof(buffer));
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_in_array = cJSON_CreateNumber(7.0);

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, num_in_array);
    num_in_object = cJSON_AddNumberToObject(root, "pi", 3.1415);
    add_ok = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    print_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 5: Validate
    (void)buffer;
    (void)print_ok;
    (void)num_in_object;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}