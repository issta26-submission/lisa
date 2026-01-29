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
//<ID> 2068
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
    cJSON *num_item = NULL;
    char *unformatted = NULL;
    char *buffered = NULL;
    double got_num = 0.0;
    int validation = 0;

    // step 2: Setup - create object and add a numeric member
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "value", 42.5);

    // step 3: Operate - produce two forms of serialized JSON
    unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    buffered = cJSON_PrintBuffered((const cJSON *)root, 128, 0);

    // step 4: Operate - retrieve number and perform basic validation checks
    got_num = cJSON_GetNumberValue((const cJSON *)num_item);
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(buffered != NULL);
    validation ^= (int)(unformatted != NULL && unformatted[0] == '{');
    validation ^= (int)(got_num == 42.5);

    // step 5: Cleanup - free allocated strings and delete the JSON tree
    cJSON_free(unformatted);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // step 6: Finalize - store a printable result and return
    buf[0] = (char)(validation + '0');

    // API sequence test completed successfully
    return 66;
}