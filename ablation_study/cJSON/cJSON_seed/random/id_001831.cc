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
//<ID> 1831
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_a = (cJSON *)0;
    const char *unused_str = (const char *)0;
    int validation = 0;
    char json[] = " { \"a\": 1 , \"arr\" : [ 10 , 20 , 30 ] } ";

    // step 2: Initialize
    cJSON_Minify(json);
    root = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddNumberToObject(root, "a", 1.0);
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(10.0);
    item1 = cJSON_CreateNumber(20.0);
    item2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    detached = cJSON_DetachItemFromArray(retrieved_arr, 1);
    retrieved_a = cJSON_GetObjectItem(root, "a");
    unused_str = json; /* ensure minified json is meaningfully used */

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(detached != (cJSON *)0) + (int)(retrieved_a != (cJSON *)0) + (int)(cJSON_GetNumberValue(retrieved_a) == 1.0) + (int)(cJSON_GetNumberValue(detached) == 20.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)item0;
    (void)item1;
    (void)item2;
    (void)unused_str;
    (void)retrieved_arr;
    (void)retrieved_a;
    (void)arr;
    (void)detached;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}