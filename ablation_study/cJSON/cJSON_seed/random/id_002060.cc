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
//<ID> 2060
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
    cJSON *num_pi = NULL;
    cJSON *num_answer = NULL;
    char *unformatted = NULL;
    char *buffered = NULL;
    double got_pi = 0.0;
    double got_answer = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_answer = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Configure
    unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    buffered = cJSON_PrintBuffered((const cJSON *)root, 32, 0);

    // step 4: Operate
    got_pi = cJSON_GetNumberValue((const cJSON *)num_pi);
    got_answer = cJSON_GetNumberValue((const cJSON *)num_answer);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_pi != NULL);
    validation ^= (int)(num_answer != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(buffered != NULL);
    validation ^= (int)(got_pi > 3.14);
    validation ^= (int)(got_answer == 42.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}